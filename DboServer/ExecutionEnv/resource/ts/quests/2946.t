CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 2946;
	title = 294602;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "1;0;252;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 294607;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 56;
			}
			CDboTSClickNPC
			{
				npcidx = "4481204;";
			}
			CDboTSCheckClrQst
			{
				and = "2936;";
				flink = 0;
				flinknextqid = "2947;";
				not = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 294609;
				ctype = 1;
				idx = 4481204;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 294608;
				area = 294601;
				goal = 294604;
				m0fz = "-3925.770020";
				m0widx = 1;
				scitem = -1;
				sort = 294605;
				stype = 1;
				m0fy = "0.000000";
				m0pat = 0;
				grade = 132203;
				m0fx = "3700.459961";
				m0ttip = 294615;
				rwd = 100;
				taid = 1;
				title = 294602;
				gtype = 1;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 294614;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 294601;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4511313;";
			}
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
			cont = 294608;
			gtype = 1;
			oklnk = 2;
			area = 294601;
			goal = 294604;
			sort = 294605;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 294602;
		}
	}
}

