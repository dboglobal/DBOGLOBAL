CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 7;
	rq = 0;
	tid = 2240;
	title = 224002;

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
				conv = 224009;
				ctype = 1;
				idx = 5061118;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 224008;
				gtype = 1;
				area = 224001;
				goal = 224004;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 224005;
				stype = 1;
				taid = 1;
				title = 224002;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 224007;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 51;
			}
			CDboTSClickNPC
			{
				npcidx = "5061118;";
			}
			CDboTSCheckClrQst
			{
				and = "2239;";
				flink = 1;
				flinknextqid = "2241;";
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
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 224008;
			gtype = 1;
			oklnk = 2;
			area = 224001;
			goal = 224004;
			sort = 224005;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 224002;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 224014;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 224001;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "5101203;";
			}
		}
	}
}

