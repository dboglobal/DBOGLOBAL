CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 6;
	rq = 0;
	tid = 1527;
	title = 152702;

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
				conv = 152709;
				ctype = 1;
				idx = 3242201;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 152708;
				gtype = 1;
				area = 152701;
				goal = 152704;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 152705;
				stype = 1;
				taid = 1;
				title = 152702;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 152714;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 152701;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "2801110;";
			}
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
			cont = 152708;
			gtype = 1;
			oklnk = 2;
			area = 152701;
			goal = 152704;
			sort = 152705;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 152702;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 152707;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 48;
			}
			CDboTSClickNPC
			{
				npcidx = "3242201;";
			}
			CDboTSCheckClrQst
			{
				and = "1514;";
				flink = 1;
				flinknextqid = "1528;";
				not = 0;
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
	}
}

