CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 2913;
	title = 291302;

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
			stdiag = 291307;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 55;
			}
			CDboTSClickNPC
			{
				npcidx = "4511312;";
			}
			CDboTSCheckClrQst
			{
				and = "2912;";
				flink = 0;
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
				conv = 291309;
				ctype = 1;
				idx = 4511312;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 291308;
				area = 291301;
				goal = 291304;
				m0fz = "-3238.489990";
				m0widx = 1;
				scitem = -1;
				sort = 291305;
				stype = 2;
				m0fy = "0.000000";
				m0pat = 0;
				grade = 132203;
				m0fx = "2306.780029";
				m0ttip = 291315;
				rwd = 100;
				taid = 1;
				title = 291302;
				gtype = 1;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 291314;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 291301;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4511214;";
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
			cont = 291308;
			gtype = 1;
			oklnk = 2;
			area = 291301;
			goal = 291304;
			sort = 291305;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 291302;
		}
	}
}

