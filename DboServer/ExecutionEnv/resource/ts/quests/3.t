CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 0;
	rq = 0;
	tid = 3;
	title = 302;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActPortal
			{
				dx = "-0.751000";
				px = "4474.109863";
				widx = 1;
				dy = "0.000000";
				py = "-42.000000";
				taid = 3;
				type = 0;
				dz = "-0.661000";
				pz = "3958.379883";
			}
			CDboTSActRegQInfo
			{
				cont = 308;
				gtype = 1;
				area = 301;
				goal = 304;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 305;
				stype = 1;
				taid = 1;
				title = 302;
			}
			CDboTSActNPCConv
			{
				conv = 309;
				ctype = 1;
				idx = 3221202;
				taid = 2;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 308;
			gtype = 1;
			oklnk = 2;
			area = 301;
			goal = 304;
			sort = 305;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 302;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 314;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 301;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4181101;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 307;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 1;
			}
			CDboTSCheckPCRace
			{
				raceflg = 1;
			}
			CDboTSCheckClrQst
			{
				and = "2;";
				flink = 1;
				flinknextqid = "189;";
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "3221202;";
			}
			CDboTSCheckPCCls
			{
				clsflg = 2;
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
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
	}
}

