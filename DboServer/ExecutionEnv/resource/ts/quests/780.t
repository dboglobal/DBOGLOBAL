CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 3;
	rq = 0;
	tid = 780;
	title = 78002;

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
				conv = 78009;
				ctype = 1;
				idx = 1311204;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 78008;
				gtype = 1;
				area = 78001;
				goal = 78004;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 78005;
				stype = 2;
				taid = 1;
				title = 78002;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 78007;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 26;
			}
			CDboTSClickNPC
			{
				npcidx = "1311204;";
			}
			CDboTSCheckClrQst
			{
				and = "779;";
				flink = 1;
				not = 0;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 78008;
			gtype = 1;
			oklnk = 2;
			area = 78001;
			goal = 78004;
			sort = 78005;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 78002;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 78014;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 78001;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3332203;";
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
	}
}

