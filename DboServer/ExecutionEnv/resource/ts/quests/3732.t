CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 3732;
	title = 373202;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 253;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 373209;
				ctype = 1;
				idx = 3204301;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 373208;
				gtype = 1;
				area = 373201;
				goal = 373204;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 373205;
				stype = 2;
				taid = 1;
				title = 373202;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 373208;
			gtype = 1;
			oklnk = 2;
			area = 373201;
			goal = 373204;
			sort = 373205;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 373202;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 373214;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 373201;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "1752105;";
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "2;0;1;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 373207;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 60;
			}
			CDboTSClickNPC
			{
				npcidx = "3204301;";
			}
			CDboTSCheckClrQst
			{
				and = "3731;";
				flink = 0;
				flinknextqid = "3733;";
				not = 0;
			}
		}
	}
}

