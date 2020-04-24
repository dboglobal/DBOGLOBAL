CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 6;
	rq = 0;
	tid = 1170;
	title = 117002;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 117009;
				ctype = 1;
				idx = 3173110;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 117008;
				gtype = 1;
				area = 117001;
				goal = 117004;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 117005;
				stype = 2;
				taid = 1;
				title = 117002;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 117007;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 47;
			}
			CDboTSClickNPC
			{
				npcidx = "3173110;";
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "1171;";
				not = 0;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 117008;
			gtype = 1;
			oklnk = 2;
			area = 117001;
			goal = 117004;
			sort = 117005;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 117002;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 117014;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 117001;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "5533301;";
			}
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
	}
}

