CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 7;
	rq = 0;
	tid = 2290;
	title = 229002;

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
				conv = 229009;
				ctype = 1;
				idx = 1491102;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 229008;
				gtype = 1;
				area = 229001;
				goal = 229004;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 229005;
				stype = 2;
				taid = 1;
				title = 229002;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 229007;
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
				npcidx = "1491102;";
			}
			CDboTSCheckClrQst
			{
				and = "2289;";
				flink = 1;
				flinknextqid = "2291;";
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
			cont = 229008;
			gtype = 1;
			oklnk = 2;
			area = 229001;
			goal = 229004;
			sort = 229005;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 229002;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 229014;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 229001;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3242119;";
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

