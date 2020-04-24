CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 8;
	rq = 0;
	tid = 2520;
	title = 252002;

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
				conv = 252009;
				ctype = 1;
				idx = 3181108;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 252008;
				gtype = 1;
				area = 252001;
				goal = 252004;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 252005;
				stype = 2;
				taid = 1;
				title = 252002;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 252007;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 53;
			}
			CDboTSClickNPC
			{
				npcidx = "3181108;";
			}
			CDboTSCheckClrQst
			{
				and = "2519;";
				flink = 1;
				flinknextqid = "2521;";
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
			cont = 252008;
			gtype = 1;
			oklnk = 2;
			area = 252001;
			goal = 252004;
			sort = 252005;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 252002;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 252014;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 252001;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "6111106;";
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

