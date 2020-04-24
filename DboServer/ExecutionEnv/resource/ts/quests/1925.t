CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 0;
	rq = 0;
	tid = 1925;
	title = 192502;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 100;
			prelnk = "2;";

			CDboTSActPortal
			{
				dx = "-0.751000";
				px = "4474.109863";
				widx = 1;
				dy = "0.000000";
				py = "-42.000000";
				taid = 1;
				type = 0;
				dz = "-0.661000";
				pz = "3958.379883";
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 4;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 192509;
				ctype = 1;
				idx = 3171106;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 192508;
				gtype = 1;
				area = 192501;
				goal = 192504;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 192505;
				stype = 1;
				taid = 1;
				title = 192502;
			}
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
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 192507;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 1;
			}
			CDboTSCheckPCCls
			{
				clsflg = 1;
			}
			CDboTSCheckPCRace
			{
				raceflg = 1;
			}
			CDboTSCheckClrQst
			{
				and = "1924;";
				flink = 1;
				flinknextqid = "189;";
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "3171106;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 192508;
			gtype = 1;
			oklnk = 2;
			area = 192501;
			goal = 192504;
			sort = 192505;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 192502;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 192514;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 192501;
			rwdtype = 0;
			ltime = -1;
			prelnk = "4;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4181101;";
			}
		}
	}
}

