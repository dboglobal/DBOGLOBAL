CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 23;
	rq = 0;
	tid = 6904;
	title = 690402;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContStart
		{
			cid = 0;
			stdiag = 690407;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 40;
			}
			CDboTSCheckPCRace
			{
				raceflg = 7;
			}
			CDboTSClickNPC
			{
				npcidx = "6091101;";
			}
			CDboTSCheckClrQst
			{
				and = "6903;";
				flink = 0;
				not = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 253;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 690409;
				ctype = 1;
				idx = 6091101;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 690408;
				gtype = 1;
				area = 690401;
				goal = 690404;
				m0fz = "-513.359985";
				m0widx = 1;
				sort = 690405;
				stype = 1;
				m0fy = "-60.400002";
				m0pat = 2;
				grade = -1;
				m0fx = "5900.149902";
				m0ttip = 690415;
				rwd = 100;
				taid = 1;
				title = 690402;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;2;";
			type = 0;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 690408;
			gtype = 1;
			oklnk = 2;
			area = 690401;
			goal = 690404;
			sort = 690405;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 690402;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 690414;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 690401;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "6091101;";
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
	}
}

