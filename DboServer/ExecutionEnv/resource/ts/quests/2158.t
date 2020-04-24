CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 7;
	rq = 0;
	tid = 2158;
	title = 215802;

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
				conv = 215809;
				ctype = 1;
				idx = 4511111;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 215808;
				gtype = 1;
				area = 215801;
				goal = 215804;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 215805;
				stype = 1;
				taid = 1;
				title = 215802;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 215807;
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
				npcidx = "4511111;";
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "2159;";
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
			cont = 215808;
			gtype = 1;
			oklnk = 2;
			area = 215801;
			goal = 215804;
			sort = 215805;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 215802;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 215814;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 215801;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4371502;";
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

