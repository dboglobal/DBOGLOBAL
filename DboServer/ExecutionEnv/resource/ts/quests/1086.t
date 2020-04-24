CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = 4;
	rq = 0;
	tid = 1086;
	title = 108602;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 100;

			CDboTSRcvSvrEvt
			{
				id = 256;
			}
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
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;252;1;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 108607;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 40;
			}
			CDboTSClickObject
			{
				objidx = "47;";
				widx = 4;
			}
			CDboTSCheckClrQst
			{
				and = "1090;";
				flink = 1;
				not = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActRegQInfo
			{
				cont = 108608;
				gtype = 3;
				area = 108601;
				goal = 108604;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 108605;
				stype = 2;
				taid = 1;
				title = 108602;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 108614;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 108601;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4071116;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 108608;
			gtype = 3;
			oklnk = 2;
			area = 108601;
			goal = 108604;
			sort = 108605;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 108602;
		}
	}
	CNtlTSGroup
	{
		gid = 254;

		CDboTSContStart
		{
			cid = 0;
			stdiag = 108607;
			nolnk = 255;
			rm = 0;
			yeslnk = 2;
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 255;
			nextlnk = 254;
			prelnk = "0;";

			CDboTSActItem
			{
				iidx0 = 99071;
				stype0 = 1;
				taid = 1;
				type = 1;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "2;";
			type = 0;
		}
	}
}

