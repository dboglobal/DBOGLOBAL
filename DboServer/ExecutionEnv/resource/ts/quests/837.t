CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 3;
	rq = 0;
	tid = 837;
	title = 83702;

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
				conv = 83709;
				ctype = 1;
				idx = 1653101;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 83708;
				gtype = 1;
				area = 83701;
				goal = 83704;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 83705;
				stype = 2;
				taid = 1;
				title = 83702;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 83714;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 83701;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "1653108;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 83708;
			gtype = 1;
			oklnk = 2;
			area = 83701;
			goal = 83704;
			sort = 83705;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 83702;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 83707;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 29;
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "838;";
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "1653101;";
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
	}
}

