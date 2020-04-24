CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 3;
	rq = 0;
	tid = 696;
	title = 69602;

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
				conv = 69609;
				ctype = 1;
				idx = 1351202;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 69608;
				gtype = 1;
				area = 69601;
				goal = 69604;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 69605;
				stype = 2;
				taid = 1;
				title = 69602;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 69607;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 23;
			}
			CDboTSClickNPC
			{
				npcidx = "1351202;";
			}
			CDboTSCheckClrQst
			{
				and = "695;";
				flink = 1;
				flinknextqid = "697;";
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
			cont = 69608;
			gtype = 1;
			oklnk = 2;
			area = 69601;
			goal = 69604;
			sort = 69605;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 69602;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 69614;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 69601;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "1351208;";
			}
		}
	}
}

