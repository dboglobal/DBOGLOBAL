CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 2;
	rq = 0;
	tid = 403;
	title = 40302;

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
				conv = 40309;
				ctype = 1;
				idx = 4371202;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 40308;
				gtype = 1;
				area = 40301;
				goal = 40304;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 40305;
				stype = 2;
				taid = 1;
				title = 40302;
			}
			CDboTSActPortal
			{
				dx = "-0.496000";
				px = "5770.500000";
				widx = 1;
				dy = "0.000000";
				py = "-74.599998";
				taid = 255;
				type = 0;
				dz = "-0.868000";
				pz = "-4035.729980";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 40307;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckPCRace
			{
				raceflg = 4;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 1;
			}
			CDboTSClickNPC
			{
				npcidx = "4371202;";
			}
			CDboTSCheckClrQst
			{
				and = "402;";
				flink = 1;
				flinknextqid = "580;";
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
			cont = 40308;
			gtype = 1;
			oklnk = 2;
			area = 40301;
			goal = 40304;
			sort = 40305;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 40302;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 40314;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 40301;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "7711101;";
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

