CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 7;
	rq = 0;
	tid = 2266;
	title = 226602;

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
				conv = 226609;
				ctype = 1;
				idx = 8031302;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 226608;
				gtype = 1;
				area = 226601;
				goal = 226604;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 226605;
				stype = 1;
				taid = 1;
				title = 226602;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 226614;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 226601;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "5101303;";
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
			cont = 226608;
			gtype = 1;
			oklnk = 2;
			area = 226601;
			goal = 226604;
			sort = 226605;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 226602;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 226607;
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
				npcidx = "8031302;";
			}
			CDboTSCheckClrQst
			{
				and = "2265;";
				flink = 1;
				not = 0;
			}
		}
	}
}

