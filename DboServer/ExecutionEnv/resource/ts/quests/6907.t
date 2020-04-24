CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = -1;
	rq = 1;
	tid = 6907;
	title = 690702;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;2;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 690707;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckClrQst
			{
				and = "6906;";
				flink = 0;
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "5523101;";
			}
			CDboTSCheckLvl
			{
				maxlvl = 30;
				minlvl = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 253;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActRegQInfo
			{
				cont = 690708;
				area = 690701;
				goal = 690704;
				m0fz = "2878.092041";
				m0widx = 1;
				scitem = -1;
				sort = 690705;
				stype = 128;
				m0fy = "-51.154999";
				m0pat = 2;
				grade = -1;
				m0fx = "3692.998047";
				m0ttip = 690715;
				rwd = 100;
				taid = 1;
				title = 690702;
				gtype = 1;
			}
			CDboTSActPortal
			{
				dx = "-0.138000";
				px = "3694.406006";
				widx = 1;
				dy = "0.000000";
				py = "0.000000";
				taid = 2;
				type = 1;
				dz = "-0.990000";
				pz = "2971.881104";
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
			cont = 690708;
			gtype = 1;
			oklnk = 2;
			area = 690701;
			goal = 690704;
			sort = 690705;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 690702;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 690714;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 690701;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "5523102;";
			}
		}
	}
}

